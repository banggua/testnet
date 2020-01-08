#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>

#include <uv.h>

#include "randpkg.h"

void on_read(uv_udp_t *server, ssize_t nread, const uv_buf_t *buf,
             const struct sockaddr *addr, unsigned flags);
void after_send(uv_udp_send_t *req, int status);
void send_pkg(int size);

uv_loop_t *loop;
uv_udp_t client;
sockaddr_in dest;

struct {
  std::map<int, std::string> SENTs;
  int count;
  int64_t sum;
  int64_t average;
  int i, j;
} Client;

struct send_req_t {
  uv_udp_send_t req;
  uv_buf_t buf;
};

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
  buf->base = (char *)malloc(suggested_size);
  buf->len = suggested_size;
}

void on_read(uv_udp_t *server, ssize_t nread, const uv_buf_t *buf,
             const sockaddr *addr, unsigned flags) {
  if (nread > 0) {
    int64_t ts = getts(std::string(buf->base));

    if (std::string(buf->base) == Client.SENTs[ts]) {
      Client.count++;
      Client.sum += timestamp() - ts;
      Client.average = Client.sum / Client.count;

      printf("Client RTT %d current %ld (%ld) average %ld\n", Client.count,
             timestamp() - ts, nread, Client.average);

      if (Client.i < (int)PackageSize.size() && Client.j < SendTimes) {
        Client.i += ++Client.j / SendTimes;
        Client.j = Client.j % SendTimes;
        int size = PackageSize[Client.i];

        send_pkg(size);
      }
    } else {
      fprintf(stderr, "Read error PACKAGE WRONG %lu\n", nread);
    }
  }

  if (nread < 0) {
    fprintf(stderr, "Read error %s\n", uv_err_name(nread));
    return;
  }
  free(buf->base);
}

void after_send(uv_udp_send_t *req, int status) {
  if (status) {
    fprintf(stderr, "Send error %s\n", uv_strerror(status));
  }

  uv_udp_recv_start(req->handle, alloc_buffer, on_read);
}

void send_pkg(int size) {
  int64_t ts = timestamp();
  std::string pkg = randpkg(size, ts);
  Client.SENTs[ts] = pkg;
  char *data = (char *)malloc(size);
  pkg.copy(data, size, 0);

  send_req_t *req = (send_req_t *)malloc(sizeof(send_req_t));
  req->buf = uv_buf_init(data, size);

  uv_udp_send((uv_udp_send_t *)req, &client, &req->buf, 1,
              (const sockaddr *)&dest, after_send);
}

int main() {
  Client.i = 0;
  Client.j = 0;

  loop = uv_default_loop();
  uv_udp_init(loop, &client);

  uv_ip4_addr("0.0.0.0", 7000, &dest);
  send_pkg(PackageSize[Client.i]);

  return uv_run(loop, UV_RUN_DEFAULT);
}

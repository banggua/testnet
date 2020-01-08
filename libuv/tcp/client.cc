
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <vector>

#include <pthread.h>
#include <uv.h>

#include "randpkg.h"

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
void on_close(uv_handle_t* handle);
void after_write(uv_write_t* req, int status);
void write_pkg(uv_stream_t* stream, int size);
void on_read(uv_stream_t* server, ssize_t nread, const uv_buf_t* buf);
void on_connect(uv_connect_t* connect, int status);

uv_loop_t* loop;
uv_tcp_t client;
sockaddr_in dest;

struct {
  std::map<int, std::string> SENTs;
  int count;
  int64_t sum;
  int64_t average;
  int i, j;
} Client;

typedef struct {
  uv_write_t req;
  uv_buf_t buf;
} write_req_t;

void free_write_req(uv_write_t* req) {
  write_req_t* wr = (write_req_t*)req;
  free(wr->buf.base);
  free(wr);
}

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  buf->base = (char*)malloc(suggested_size);
  buf->len = suggested_size;
}

void on_close(uv_handle_t* handle) { printf("closed."); }

void after_write(uv_write_t* req, int status) {
  if (status) {
    fprintf(stderr, "Write error %s\n", uv_strerror(status));
  }
  free_write_req(req);
}

void write_pkg(uv_stream_t* stream, int size) {
  int64_t ts = timestamp();
  std::string pkg = randpkg(size, ts);
  Client.SENTs[ts] = pkg;
  char* data = (char*)malloc(size);
  pkg.copy(data, size, 0);

  write_req_t* req = (write_req_t*)malloc(sizeof(write_req_t));
  req->buf = uv_buf_init(data, size);
  uv_write((uv_write_t*)req, stream, &req->buf, 1, after_write);
}

void on_read(uv_stream_t* server, ssize_t nread, const uv_buf_t* buf) {
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

        write_pkg(server, size);
        uv_read_start(server, alloc_buffer, on_read);
      }
    } else {
      fprintf(stderr, "Read error PACKAGE WRONG %lu\n", nread);
    }
  }
  if (nread < 0) {
    if (nread != UV_EOF) fprintf(stderr, "Read error %s\n", uv_err_name(nread));
    uv_close((uv_handle_t*)server, on_close);
  }

  free(buf->base);
}

void on_connect(uv_connect_t* connect, int status) {
  if (status < 0) {
    fprintf(stderr, "Connect error %s\n", uv_err_name(status));
    free(connect);
    return;
  }

  uv_stream_t* server = connect->handle;
  free(connect);

  int size = PackageSize[Client.i];

  write_pkg(server, size);
  uv_read_start(server, alloc_buffer, on_read);
}

int main(int argc, char** argv) {
  Client.i = 0;
  Client.j = 0;

  loop = uv_default_loop();
  uv_tcp_init(loop, &client);
  uv_tcp_keepalive(&client, 1, 60);

  uv_ip4_addr("127.0.0.1", 7000, &dest);
  uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));
  uv_tcp_connect(connect, &client, (const sockaddr*)&dest, on_connect);

  uv_run(loop, UV_RUN_DEFAULT);
}

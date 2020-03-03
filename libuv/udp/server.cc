#include <uv.h>

#include <cstdio>
#include <cstdlib>

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
void on_close(uv_handle_t *handle);
void after_send(uv_udp_send_t *req, int status);
void on_recv(uv_udp_t *client, ssize_t nread, const uv_buf_t *buf,
             const sockaddr *addr, unsigned flags);

uv_loop_t *loop;
uv_udp_t server;
sockaddr_in addr;

struct send_req_t {
  uv_udp_send_t req;
  uv_buf_t buf;
};

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
  buf->base = (char *)malloc(suggested_size);
  buf->len = suggested_size;
}

void on_close(uv_handle_t *handle) { uv_is_closing(handle); }

void after_send(uv_udp_send_t *req, int status) {
  if (status) {
    fprintf(stderr, "Send error %s\n", uv_strerror(status));
  }
  free(req);
}

void on_recv(uv_udp_t *client, ssize_t nread, const uv_buf_t *buf,
             const sockaddr *addr, unsigned flags) {
  if (nread < 0) {
    fprintf(stderr, "Read error %s\n", uv_err_name(nread));
    uv_close((uv_handle_t *)client, NULL);
    free(buf->base);
    return;
  }

  if (nread > 0) {
    printf("Server %ld\n", nread);
    send_req_t *req = (send_req_t *)malloc(sizeof(send_req_t));
    req->buf = uv_buf_init(buf->base, nread);

    uv_udp_send((uv_udp_send_t *)req, client, &req->buf, 1, addr, after_send);
  }

  return;
}

int main() {
  loop = uv_default_loop();
  uv_udp_init(loop, &server);

  uv_ip4_addr("0.0.0.0", 7000, &addr);
  uv_udp_bind(&server, (const sockaddr *)&addr, UV_UDP_REUSEADDR);
  uv_udp_recv_start(&server, alloc_buffer, on_recv);

  return uv_run(loop, UV_RUN_DEFAULT);
}

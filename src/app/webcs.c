/*
 * This file is part of AstroM-WebCS project
 * Copyright (C) 2026 Timofey Kostenok <tim_kostenok@mail.ru>
 *
 * This project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/**
 * @file webcs.c
 * @brief Implementation of the HTTP front end, built on facil.io
 *
 * Only declarations are pulled from facil.io here: `FIO_HTTP` selects the HTTP
 * module and `FIO_EXTERN` makes the API functions external instead of `static`.
 * The definitions themselves are compiled once, in `src/fio_impl.c`.
 */
#define FIO_HTTP
#define FIO_EXTERN
#include "fio-stl.h"

#include "webcs.h"

#include <stdio.h>

#include "core.h"

/** Kept so that a future stop call has something to stop. */
static fio_http_listener_s *amwcs__webcs_listener = NULL;

/**
 * @brief Answers every request with the service name and its version.
 * @param h HTTP handle of the request.
 */
static void amwcs__webcs_on_http(fio_http_s *h) {
  char body[128];
  const int len = snprintf(body,
                           sizeof(body),
                           "astrom-webcs %s\n",
                           amwcs_core_version());
  if (len < 0 || (size_t)len >= sizeof(body)) {
    fio_http_send_error_response(h, 500);
    return;
  }

  fio_http_status_set(h, 200);
  fio_http_response_header_set(h,
                               FIO_STR_INFO1("content-type"),
                               FIO_STR_INFO1("text/plain; charset=utf-8"));

  /*
   * `copy` is required: `body` lives on this stack frame, while the write is
   * handed to the reactor and completes later.
   */
  fio_http_write(h, .buf = body, .len = (size_t)len, .copy = 1, .finish = 1);
}

int amwcs_webcs_start(int port) {
  char url[64];
  const int len = snprintf(url, sizeof(url), "http://127.0.0.1:%d", port);
  if (len < 0 || (size_t)len >= sizeof(url))
    return -1;

  amwcs__webcs_listener =
      fio_http_listen(url, .on_http = amwcs__webcs_on_http);
  return amwcs__webcs_listener ? 0 : -1;
}

void amwcs_webcs_run(void) {
  /* 0 workers: the event loop runs on the calling thread, no child processes. */
  fio_io_start(0);
}

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
 * @file main.c
 * @brief Entry point of the AstroM-WebCS application
 *
 * Return codes follow the project convention: `main` returns 0 on success and
 * 1 on error, internal functions return 0 on success and -1 on failure.
 */
#include <stdio.h>

#include "core.h"
#include "daemon.h"
#include "webcs.h"

#ifndef AMWCS_DEFAULT_PORT
#define AMWCS_DEFAULT_PORT 8080
#endif

int main(void) {
  const int port = AMWCS_DEFAULT_PORT;

  if (amwcs_daemon_ping() != 0)
    fprintf(stderr, "astrom-webcs: control layer is unreachable\n");
  else
    fprintf(stderr,
            "astrom-webcs %s: control layer OK\n",
            amwcs_core_version());

  if (amwcs_webcs_start(port) != 0) {
    fprintf(stderr, "astrom-webcs: cannot listen on port %d\n", port);
    return 1;
  }

  fprintf(stderr,
          "astrom-webcs %s: listening on http://127.0.0.1:%d\n",
          amwcs_core_version(),
          port);

  amwcs_webcs_run(); /* blocks until shutdown */
  return 0;
}

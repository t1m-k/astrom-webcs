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
 * @brief Entry point of the AstroM-WebCS test binary
 *
 * Only wiring checks for now: the binary has to build and link against
 * `amwcs_core` with the RAM implementation of `daemon.h`.
 *
 * Return codes follow the project convention: `main` returns 0 on success and
 * 1 on error, helper functions return 0 on success and -1 on failure.
 */
#include <stdio.h>

#include "core.h"
#include "daemon.h"

/**
 * @brief Checks that the core reports a non-empty version.
 * @return 0 on success, -1 on failure.
 */
static int test_version_is_reported(void) {
  const char *version = amwcs_core_version();
  if (!version || !*version) {
    fprintf(stderr, "amwcs_core_version() returned an empty string\n");
    return -1;
  }
  printf("core version: %s\n", version);
  return 0;
}

/**
 * @brief Checks that the `daemon.h` seam is linked and answers.
 * @return 0 on success, -1 on failure.
 */
static int test_daemon_seam_links(void) {
  if (amwcs_daemon_ping() != 0) {
    fprintf(stderr, "amwcs_daemon_ping() reported failure\n");
    return -1;
  }
  return 0;
}

int main(void) {
  int failed = 0;
  if (test_version_is_reported() != 0)
    ++failed;
  if (test_daemon_seam_links() != 0)
    ++failed;

  if (failed) {
    fprintf(stderr, "%d test(s) failed\n", failed);
    return 1;
  }
  printf("all tests passed\n");
  return 0;
}

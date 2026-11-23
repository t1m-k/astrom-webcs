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
 * @file daemon.h
 * @brief API definitions layer for telescope control daemons
 *
 * Implementation for any target is located in `daemon.c`, e.g.
 * - `src/app/daemon.c`
 * - `src/tests/daemon.c`
 */

#ifndef H___AMWCS_DAEMON___H
#define H___AMWCS_DAEMON___H

/**
 * @brief Checks if this layer is reachable.
 * @return 0 on success, -1 on failure.
 *
 * On failure `errno` may hold the reason, as set by the underlying call; read
 * it immediately if needed.
 */
int amwcs_daemon_ping(void);

#endif /* H___AMWCS_DAEMON___H */

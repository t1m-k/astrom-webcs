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
 * @file webcs.h
 * @brief HTTP front end of the AstroM-WebCS project
 *
 * Kept free of facil.io types, so `main.c` does not need to know the library
 * exists.
 */

#ifndef H___AMWCS_WBCS___H
#define H___AMWCS_WBCS___H

/**
 * @brief Starts listening for HTTP connections.
 * @param port TCP port to listen on.
 * @return 0 on success, -1 if the listener could not be created.
 *
 * On failure `errno` may hold the reason, as set by the underlying call; read
 * it immediately if needed.
 *
 * @note Does not block. Call `amwcs_webcs_run` to enter the event loop.
 */
int amwcs_webcs_start(int port);

/**
 * @brief Runs the event loop.
 * @note Blocks until the process is asked to stop.
 */
void amwcs_webcs_run(void);

#endif /* H___AMWCS_WBCS___H */

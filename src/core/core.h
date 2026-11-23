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
 * @file core.h
 * @brief Business logic of the AstroM-WebCS project
 *
 * Linked into both the application and the test binary.
 */

#ifndef H___AMWCS_CORE___H
#define H___AMWCS_CORE___H

/**
 * @brief Returns the version of the running service.
 * @return Version string from the build system (`AMWCS_VERSION`).
 */
const char *amwcs_core_version(void);

#endif /* H___AMWCS_CORE___H */

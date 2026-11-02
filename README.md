# AstroM-WebCS
Web interface for control system of SAO RAS complex «Astro-M»

## Installing & running
Clone the repo:
```bash
git clone https://github.com/t1m-k/astrom-webcs.git -j 8 --depth 1
```
Flag `--depth 1` is optional, used not to clone all commit history.

Once successfully cloned, just do nothing, because it is developing now.

## Licensing

This project is distributed under the **GNU Affero General Public License
v3.0 or later** — see [`LICENSE`](LICENSE) file for details.

Third-party dependencies:
- Project can link against installed in your OS [OpenSSL](https://github.com/openssl/openssl/) distributed under `Apache 2.0` license
- Project uses [facil.io / cstl](https://github.com/facil-io/cstl) in the form of a single amalgamated header [`deps/fio-stl.h`](deps/fio-stl.h), which remains under its own
ISC / MIT dual license (Copyright © Boaz Segev, used with thanks). The full text of these licenses can be found below.

<details>
<summary>The MIT License</summary>

MIT License

Copyright (c) 2019-2026 Boaz Segev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
</details>
<details>
<summary>The ISC License</summary>

ISC License

Copyright (c) 2019-2026 Boaz Segev

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
</details>

## facil-io / cstl

This dependency is managed as [`deps/fio-stl.h`](deps/fio-stl.h) file which
is manually downloaded from https://raw.githubusercontent.com/facil-io/cstl/COMMIT/fio-stl.h

When updating, replace `COMMIT` with the actual target commit hash. The corresponging commit
hash and SHA-256 checksum of the file must also be updated in the [root CMakeLists.txt](CMakeLists.txt).

### Current version
- COMMIT: `3099b7f93e0f7e7f37c88ba23972658e2ac011df`
- SHA-256: `038100bb9c6a0cfc8dab8212968164e50a8813fee2c985c2498dffe871889ec7`

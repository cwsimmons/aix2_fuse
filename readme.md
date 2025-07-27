# IBM AIX 2 FUSE Filesytem

## Overview

This program allows you to mount an AIX 2 filesystem from an IBM RT under Linux.

## Usage Notes

    aix2fuse --filename <image file> --offset <offset to start of filessytem in bytes> <mount point>

* Pass `--help` to see options provided by FUSE.

## Limitations

* Read-only
* Probably lots of bugs.

## License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.

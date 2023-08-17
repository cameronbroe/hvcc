# Copyright (C) 2021-2023 Wasted Audio
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import time
# from typing import Dict, Optional


class c2juce:
    """ Generates a JUCE wrapper for a given patch.
    """

    @classmethod
    def compile(
        cls,
        # c_src_dir: str,
        # out_dir: str,
        # externs: Dict,
        # patch_name: Optional[str] = None,
        # patch_meta: Optional[Dict] = None,
        # num_input_channels: int = 0,
        # num_output_channels: int = 0,
        # copyright: Optional[str] = None,
        # verbose: Optional[bool] = False
    ):
        print("generating JUCE wrapper")
        return {
            "stage": "c2juce",
            "notifs": {
                "has_error": False,
                "exception": None,
                "warnings": [],
                "errors": [],
            }
        }
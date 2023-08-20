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

import os
import shutil
import time
import jinja2
from typing import Dict, Optional

from hvcc.generators.copyright import copyright_manager


class c2juce:
    """ Generates a JUCE wrapper for a given patch.
    """

    @classmethod
    def compile(
        cls,
        c_src_dir: str,
        out_dir: str,
        externs: Dict,
        patch_name: Optional[str] = None,
        patch_meta: Optional[Dict] = None,
        num_input_channels: int = 0,
        num_output_channels: int = 0,
        copyright: Optional[str] = None,
        verbose: Optional[bool] = False
    ) -> Dict:
        
        tick = time.time()

        out_dir = os.path.join(out_dir, "plugin")
        receiver_list = externs["parameters"]["in"]

        if patch_meta:
            patch_name = patch_meta.get("name", patch_name)
            juce_meta = patch_meta.get("juce", {})
        else:
            juce_meta = {}

        juce_path = juce_meta.get("juce_path", "")

        copyright_c = copyright_manager.get_copyright_for_c(copyright)

        try:
            # ensure that the output directory does not exist
            out_dir = os.path.abspath(out_dir)
            if os.path.exists(out_dir):
                shutil.rmtree(out_dir)

            # copy over generated C source files
            source_dir = os.path.join(out_dir, "source")
            shutil.copytree(c_src_dir, source_dir)

            # initialize the jinja template environment
            env = jinja2.Environment()

            env.loader = jinja2.FileSystemLoader(
                os.path.join(os.path.dirname(os.path.abspath(__file__)), "templates")
            )

            # Processor
            juce_h_path = os.path.join(source_dir, f"HeavyJuceProcessor_{patch_name}.h")
            with open(juce_h_path, "w") as f:
                f.write(env.get_template("HeavyJuceProcessor.h").render(
                    name=patch_name,
                    meta=juce_meta,
                    class_name=f"HeavyJuceProcessor_{patch_name}",
                    receivers=receiver_list,
                    copyright=copyright_c
                ))
            juce_cpp_path = os.path.join(source_dir, f"HeavyJuceProcessor_{patch_name}.cpp")
            with open(juce_cpp_path, "w") as f:
                f.write(env.get_template("HeavyJuceProcessor.cpp").render(
                    name=patch_name,
                    meta=juce_meta,
                    class_name=f"HeavyJuceProcessor_{patch_name}",
                    receivers=receiver_list,
                    pool_sizes_kb=externs["memoryPoolSizesKb"],
                    copyright=copyright_c
                ))

            # Parameter wrapper
            juce_h_path = os.path.join(source_dir, f"HeavyJuceParameter.h")
            with open(juce_h_path, "w") as f:
                f.write(env.get_template("HeavyJuceParameter.h").render(
                    name=patch_name,
                    meta=juce_meta,
                    class_name=f"HeavyJuceProcessor_{patch_name}",
                    receivers=receiver_list,
                    copyright=copyright_c
                ))
            juce_cpp_path = os.path.join(source_dir, f"HeavyJuceParameter.cpp")
            with open(juce_cpp_path, "w") as f:
                f.write(env.get_template("HeavyJuceParameter.cpp").render(
                    name=patch_name,
                    meta=juce_meta,
                    class_name=f"HeavyJuceProcessor_{patch_name}",
                    receivers=receiver_list,
                    copyright=copyright_c
                ))

            # CMakeLists.txt
            juce_cmake_path = os.path.join(source_dir, "../CMakeLists.txt")
            with open(juce_cmake_path, "w") as f:
                f.write(env.get_template("CMakeLists.txt").render(
                    name=patch_name,
                    meta=juce_meta
                ))

            return {
                "stage": "c2juce",
                "notifs": {
                    "has_error": False,
                    "exception": None,
                    "warnings": [],
                    "errors": []
                }
            }
        except Exception as e:
            return {
                "stage": "c2juce",
                "notifs": {
                    "has_error": True,
                    "exception": e,
                    "warnings": [],
                    "errors": [{
                        "enum": -1,
                        "message": str(e)
                    }]
                }
            }
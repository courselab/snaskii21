# Usage: python install_all.py [--password SUDO_PASSWORD (optional)]

import argparse
import os
import subprocess

def main() -> None:
    parser = argparse.ArgumentParser()
    """
    On some systems it's necessary to use a sudo command to install the packages. This requires passing the password
    as an argument to Python. This argument is optional, since some systems don't require root privileges.
    """
    parser.add_argument("--password", type=str, help="(Optional) sudo password, if it's required by your system to install packages")
    args = parser.parse_args()
    password = args.password

    # All the installation steps, as described in README.md
    commands = [
        "apt install automake autoconf -y",
        "apt install libncurses5-dev -y",
        "./autogen.sh",
        "./configure",
        "make",
        "make install"
    ]
    
    for command in commands:
        # The command to run is different if root privileges are required.
        command_to_run = f"echo {password} | sudo -S {command}" if password is not None else command
        subprocess.run(command_to_run, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)

if __name__ == "__main__":
    if os.name == "posix":
        main()
    else:
        print(f"Currently, non-POSIX systems are not supported")
import argparse
import os
import subprocess

def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--password", type=str, help="(Optional) sudo password, if it's required by your system to install packages")
    args = parser.parse_args()
    password = args.password

    commands = [
        "apt install automake autoconf -y",
        "apt install libncurses5-dev -y",
        "./autogen.sh",
        "./configure",
        "make",
        "make install"
    ]
    
    for command in commands:
        command_to_run = f"echo {password} | sudo -S {command}" if password is not None else command
        subprocess.run(command_to_run, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)

if __name__ == "__main__":
    main()
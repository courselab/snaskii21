import subprocess

def main() -> None:    
    commands = [
        "apt install automake autoconf -y",
        "apt install libncurses5-dev -y",
        "./autogen.sh",
        "./configure",
        "make",
        "make install"
    ]
    
    for command in commands:
        subprocess.run(command.split(), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
        
if __name__ == "__main__":
    main()
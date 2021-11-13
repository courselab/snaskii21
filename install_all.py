import subprocess

def main() -> None:
    subprocess.run("apt install automake autoconf -y".split(), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
    subprocess.run("apt install libncurses5-dev -y".split(), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
    subprocess.run("./autogen.sh".split(), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
    subprocess.run("./configure".split(), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
    subprocess.run("make", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
    subprocess.run("make install", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
    
if __name__ == "__main__":
    main()
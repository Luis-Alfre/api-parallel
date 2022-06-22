import subprocess


def runGenetic():
    #subprocess.run(["ls"])
    subprocess.run(["mpirun", "-oversubscribe", "-np", "4", "islas"])


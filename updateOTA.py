import git
import os
import subprocess

absPath = os.path.abspath(os.getcwd())+"\\esp32_github_ota"

if os.path.exists(absPath+"/esp32_github_ota.ino.esp32.bin"):
    os.remove(absPath+"/fw.bin")
    os.rename(absPath+"/esp32_github_ota.ino.esp32.bin", absPath+"/fw.bin")
    versionFile = open(absPath+"/bin_version.txt","r")
    version=float(versionFile.readline())+0.1
    versionFile = open(absPath+"/bin_version.txt","w")
    versionFile.write(str("{:.1f}".format(version)))
    versionFile.close()
    os.system("comdGit.bat V"+str("{:.1f}".format(version)))
else: 
    print("--------------------No se ha encontrado el binario compilado\n"+
    "--------------------En Arduino ve a: Programa>Exportar Binarios compilados")
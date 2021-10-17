import git
import os
import subprocess

absPath = "D:/Users/Roberto/Desktop/UPM/1Cuatrimestre/Cuarto/SBC/Arduino/ESP32_AutoUpdateOTA/"
binPath='ESP32_AutoUpdateOTA/esp32_github_ota/esp32_github_ota.ino.esp32.bin'

#if os.path.exists(binPath):
#   os.remove("ESP32_AutoUpdateOTA/esp32_github_ota/fw.bin")
#   os.rename(binPath, "ESP32_AutoUpdateOTA/esp32_github_ota/fw.bin")
versionFile = open(absPath+"esp32_github_ota/bin_version.txt","r")
version=float(versionFile.readline())+0.1
versionFile = open(absPath+"esp32_github_ota/bin_version.txt","w")
versionFile.write(str("{:.1f}".format(version)))
versionFile.close()
os.system(absPath+"/actualizarOTA.bat V"+str("{:.1f}".format(version)))
#repo = git.Repo("ESP32_AutoUpdateOTA")
#repo.git.add('--all')
#repo.git.commit('-m', 'V'+str("{:.1f}".format(version)),author='rcarreror@gmail.com')
#repo.git.push()
#else: 
#    print("--------------------No se ha encontrado el binario compilado\n--------------------En Arduino ve a: Programa>Exportar Binarios compilados")
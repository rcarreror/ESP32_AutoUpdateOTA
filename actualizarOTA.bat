del esp32_github_ota\fw.bin
ren esp32_github_ota\esp32_github_ota.ino.esp32.bin fw.bin
echo %1 > esp32_github_ota\bin_version.txt
git add .
git commit -m %1
git push
	
import os
import sys
import subprocess

membranepath = sys.argv[2];
nucleipath = sys.argv[3];

membranelisting = os.listdir(membranepath);
nucleilisting = os.listdir(nucleipath);

for membrane in  membranelisting:
  for nuclei in nucleilisting:
    subprocess.call([sys.argv[1], membranepath + membrane, nucleipath + nuclei]);

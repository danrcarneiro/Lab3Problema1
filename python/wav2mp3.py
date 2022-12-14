import os
from pydub import AudioSegment

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)

files = ['music', 'echo_float', 'flanger_float']

for f in files:
  sound = AudioSegment.from_wav("../assets/" + f + ".wav")
  sound.export("../assets/" + f + ".mp3", format="mp3")
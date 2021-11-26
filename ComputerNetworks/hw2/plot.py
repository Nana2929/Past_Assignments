import matplotlib.pyplot as plt
import pandas as pd
filepath = "/Users/yangqingwen/Desktop/110-1課程/電網導/Hw2/timerlog_const_15"
df = pd.read_csv(filepath)
# print(df["estRTT"])
suffix = filepath.split("/")[-1][8:]
print(suffix)
Title = f"ERTT plot of timerlog-{suffix}"
plt.title(Title)

# ending time
df["estRTT"].iloc[-1] = (df["estRTT"].iloc[-2])
df["sampleRTT"].iloc[-1] = (df["sampleRTT"].iloc[-2])

# plt.plot(df["time"], df["estRTT"], label = "estimated RTT", color = "black")
plt.plot(df["time"], df["sampleRTT"], label = "sample RTT", color = "skyblue")
################
plt.plot(df["time"], [15]*len(df), label = "const timeout value", color = "pink")
################

plt.legend()
plt.xlabel('time')
plt.ylabel('RTT')
# plt.show()
plt.savefig("/Users/yangqingwen/Desktop/110-1課程/電網導/Hw2/figs/"+\
'_'.join(Title.split(" "))+".jpg")
import pandas as pd
import matplotlib.pyplot as plt

df1 = pd.read_csv("../data/mutation_rate_10_swaps.csv")
df2 = pd.read_csv("../data/mutation_rate_100_swaps.csv")
df3 = pd.read_csv("../data/mutation_rate_single_swap.csv")

WINDOW_SIZE = 5

# center=True: wartość dla p to średnia z pomiarów p-10..p+10, bez przesunięcia osi
df1["smooth"] = df1["result"].rolling(window=WINDOW_SIZE, center=True).mean()
df2["smooth"] = df2["result"].rolling(window=WINDOW_SIZE, center=True).mean()
df3["smooth"] = df3["result"].rolling(window=WINDOW_SIZE, center=True).mean()

plt.figure(figsize=(10, 6))

# Surowe pomiary w tle, żeby było widać skalę rozrzutu losowego
plt.plot(df1["mutation_probability"], df1["result"], color="C0", linewidth=1, alpha=0.3, marker="o", markersize=3)
plt.plot(df2["mutation_probability"], df2["result"], color="C1", linewidth=1, alpha=0.3, marker="o", markersize=3)
plt.plot(df3["mutation_probability"], df3["result"], color="C2", linewidth=1, alpha=0.3, marker="o", markersize=3)

plt.plot(
    df1["mutation_probability"],
    df1["smooth"],
    color="C0",
    label="10 zamian pozycji",
    linewidth=2
)

plt.plot(
    df2["mutation_probability"],
    df2["smooth"],
    color="C1",
    label="100 zamian pozycji",
    linewidth=2
)

plt.plot(
    df3["mutation_probability"],
    df3["smooth"],
    color="C2",
    label="pojedyncza zamiana pozycji",
    linewidth=2
)

plt.xlabel("Prawdopodobieństwo mutacji [%]", fontsize=12)
plt.ylabel("Średnia nieliniowość", fontsize=12)
# plt.title("Zależność nieliniowości od prawdopodobieństwa mutacji")

plt.grid(True)
plt.plot([], [], color="gray", linewidth=1, alpha=0.5, marker="o", markersize=3, label="pomiary bez wygładzania")
plt.legend(title=f"Średnia krocząca W={WINDOW_SIZE}", loc="upper center", bbox_to_anchor=(0.5, -0.12), ncol=4)

plt.tight_layout()

plt.savefig("nieliniowość_prawdopodobienstwo_mutacji.png")

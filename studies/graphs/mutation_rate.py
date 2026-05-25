import pandas as pd
import matplotlib.pyplot as plt

df1 = pd.read_csv("../data/mutation_rate_10_swaps.csv")
df2 = pd.read_csv("../data/mutation_rate_100_swaps.csv")
df3 = pd.read_csv("../data/mutation_rate_single_swap.csv")

WINDOW_SIZE = 5  

df1["smooth"] = df1["result"].rolling(window=WINDOW_SIZE).mean()
df2["smooth"] = df2["result"].rolling(window=WINDOW_SIZE).mean()
df3["smooth"] = df3["result"].rolling(window=WINDOW_SIZE).mean()

plt.figure(figsize=(10, 6))

plt.plot(
    df1["mutation_probability"],
    df1["smooth"],
    label="10 zamian pozycji",
    linewidth=2
)

plt.plot(
    df2["mutation_probability"],
    df2["smooth"],
    label="100 zamian pozycji",
    linewidth=2
)

plt.plot(
    df3["mutation_probability"],
    df3["smooth"],
    label="pojedyncza zamiana pozycji",
    linewidth=2
)

plt.xlabel("Prawdopodobieństwo mutacji", fontsize=12)
plt.ylabel("Średnia krocząca nieliniowości W=5", fontsize=12)
plt.title("Zależność nieliniowości od prawdopodobieństwa mutacji [%]")

plt.grid(True)
plt.legend()

plt.tight_layout()

plt.savefig("nieliniowość_prawdopodobienstwo_mutacji.png")

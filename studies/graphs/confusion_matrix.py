import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("../results.csv")

matrix = df.pivot(
    index="iterations",
    columns="populationSize",
    values="result"
)

plt.imshow(matrix.values, origin="lower", aspect="auto")
plt.colorbar(label="result")

plt.xticks(range(len(matrix.columns)), matrix.columns)
plt.yticks(range(len(matrix.index)), matrix.index)

plt.xlabel("populationSize")
plt.ylabel("iterations")
plt.title("Result matrix")

plt.show()
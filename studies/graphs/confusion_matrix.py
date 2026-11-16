import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("../data/heat_map.csv")

matrix = df.pivot(
    index="iterations",
    columns="populationSize",
    values="result"
)

plt.imshow(matrix.values, origin="lower", aspect="auto")
plt.colorbar(label="Średnia nieliniowość")

plt.xticks(range(len(matrix.columns)), matrix.columns)
plt.yticks(range(len(matrix.index)), matrix.index)

plt.xlabel("Rozmiar populacji")
plt.ylabel("Ilość iteracji")
# plt.title("Result matrix")

# plt.show()
plt.savefig("conf_matrix.jpg")
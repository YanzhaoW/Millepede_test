import json

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns

offset_filename = "offsets.json"
result_filename = "millepede.res"


def parse_result(filename: str):
    ids = np.array([], dtype=int)
    vals = np.array([], dtype=float)
    errs = np.array([], dtype=float)
    with open(filename, "r", encoding="utf-8") as res_file:
        res_file.readline()
        for line in res_file.readlines():
            strs = line.split()
            ids = np.append(ids, int(strs[0]))
            vals = np.append(vals, float(strs[1]))
            # if len(strs) > 3:
            #     err = float(strs[-1])
            # else:
            #     err = 0.0
            # errs = np.append(errs, err)
    # dataframe = pd.DataFrame(data = {"id": ids, "val": vals, "err": errs})
    dataframe = pd.DataFrame(data={"id": ids, "val": vals, "label": "res"})
    return dataframe


if __name__ == "__main__":
    with open(offset_filename, "r", encoding="utf-8") as file:
        offsets = json.load(file)

    df_offsets = pd.DataFrame(data={"id": np.arange(1, len(offsets) + 1), "val": offsets, "label": "offsets"})
    df_res = parse_result(result_filename)

    # print(df_res)
    sns.scatterplot(data=pd.concat([df_offsets, df_res]), x="id", y="val", hue="label", style = 'label', size = 'label',size_order = ('offsets', 'res'), sizes = (40, 40))
    # plt.errorbar(df_res['id'], df_res['val'], yerr = df_res['err'], linestyle = 'None', capsize = 5.0)
    plt.show()

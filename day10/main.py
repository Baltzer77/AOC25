import numpy as np
import re
from scipy.optimize import milp, LinearConstraint, Bounds

def read_machine():
    line = input()
    if line == "":
        return None, None
    
    button_strs = re.findall(r"\(([^\)]+)\)", line)
    buttons = []
    for button_str in button_strs:
        indexes = [int(i) for i in button_str.split(",")]
        buttons.append(indexes)

        jolt_str = re.search(r"\{([^\}]+)\}", line)
    jolts = [int(j) for j in jolt_str.group(1).split(",")]
    return buttons, jolts


def solveForPresses(buttons, jolts):
    n = len(jolts)
    m = len(buttons)

    A = np.zeros((n,m), dtype=int)
    for i, button_indexes in enumerate(buttons):
        A[button_indexes, i] = 1
    
    jolts = np.array(jolts, dtype=float)
    lc = LinearConstraint(A, lb=jolts, ub=jolts) # we need an exact result
    
    bounds = Bounds(lb=np.zeros(m), ub=np.full(m, np.inf))
    c= np.ones(m,dtype=float)
    integrality = np.ones_like(c) # all need to be ints
    res = milp(
        c=c,
        integrality=integrality,
        constraints=lc,
        bounds=bounds
    )
    return int(res.fun)

total_presses = 0
while True:
    buttons, jolts = read_machine()
    if buttons == None:
        break
    total_presses += solveForPresses(buttons, jolts)

print(total_presses)


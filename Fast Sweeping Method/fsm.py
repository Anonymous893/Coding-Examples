# Implementation of the Fast Sweeping Method authored by Hongkai Zhao
# Code base from Dr. Triet Le, with my changes to read the data from a generated tif file

import random
from typing import Dict
import itertools
from PIL import Image

import numpy as np
import os
import matplotlib
import richdem as rd 
from osgeo import gdal

INF = 1e10


import sys
sys.setrecursionlimit(10000)


def fsm2d(u, f, h=1, eps=1e-5):
    # ----------------------------------------------------------------
    # Inputs: 1) u is the initial cost: u(gamma) = 0 and INF elsewhere
    #         2) f is the cost function
    #         3) h = width of grid size
    # Output: 1) u = updated optimal cost function,
    #            i.e. solution of |\nabla u| = f with u(gamma) = 0.
    # ----------------------------------------------------------------
    [ny, nx] = f.shape
    y_sweeps = [
        range(0, ny),
        range(0, ny),
        range(ny - 1, -1, -1),
        range(ny - 1, -1, -1),
    ]
    x_sweeps = [
        range(0, nx),
        range(nx - 1, -1, -1),
        range(0, nx),
        range(nx - 1, -1, -1),
    ]
    # --------------------------------------------
    w = np.zeros((ny, nx))

    def update_u(i, j, u_ymin, u_xmin):
        u_new = 0
        if np.abs(u_ymin - u_xmin) < f[i, j] * h:
            u_new = (
                u_ymin
                + u_xmin
                + np.sqrt(2.0 * f[i, j] ** 2 * h**2 - (u_ymin - u_xmin) ** 2)
            ) * 0.5
        else:
            u_new = np.minimum(u_ymin, u_xmin) + f[i, j] * h
        u_new = np.minimum(u[i, j], u_new)
        w[i, j] = np.abs(u[i, j] - u_new)
        u[i, j] = u_new

    # ---------------------------------------------
    # ---------------------------------------------

    def get_u_min(i, j):
        if i == 0:
            u_ymin = u[i + 1, j]
        elif i == ny - 1:
            u_ymin = u[i - 1, j]
        else:
            u_ymin = np.minimum(u[i - 1, j], u[i + 1, j])
        if j == 0:
            u_xmin = u[i, j + 1]
        elif j == nx - 1:
            u_xmin = u[i, j - 1]
        else:
            u_xmin = np.minimum(u[i, j - 1], u[i, j + 1])
        return [u_ymin, u_xmin]

    # ------------------------------------------------
    # --- start sweeping -----------------------------
    k = 0
    while True:
        print('k = ', k)
        k += 1
        for n in range(4):
            for i in y_sweeps[n]:
                for j in x_sweeps[n]:
                    [u_ymin, u_xmin] = get_u_min(i, j)
                    update_u(i, j, u_ymin, u_xmin)
        if w.max() < eps:
            break
        for n in range(4):
            for j in x_sweeps[n]:
                for i in y_sweeps[n]:
                    [u_ymin, u_xmin] = get_u_min(i, j)
                    update_u(i, j, u_ymin, u_xmin)
        if w.max() < eps:
            break
    # -------------------------------------------------
    return u


# -------------------------------------------------

# ------------------------------------------------


def path_gen(u, gamma, omega, obstacles=None, act_type="relu", rnd=True):
    # Inputs: 
    #   1) u = optimal cost computed from fsm_2d
    #   2) gamma = source
    #   3) omega = destination
    #   4) obstacles (optional)
    #   5) activation function determining probability of marching backward
    #   6) if rnd = True, then select neighbor based on probability 
    # Output:
    #   1) a path from gamma to omega. it is optimal if rnd = False and
    #   sub-optimal if rnd = True.
    # -----------------------------------------------
    def path_gen_recur(x0, path0):
        # if has reached the source (target current location)
        if x0 == gamma:
            return path0
        else:
            pos0 = select_dir(u, x0, obstacles=obstacles, act_type=act_type, rnd=rnd)
            if pos0 is None:
                return None
            else:
                path0.insert(0, pos0)
                return path_gen_recur(pos0, path0)

    path = path_gen_recur(omega, [omega])

    return path


# ----------------------------------------------


def neighbors(x, shape, obstacles=None):
    if obstacles is None:
        obstacles = []
    ny, nx = shape
    i = x[0]
    j = x[1]
    N_ij = [
        (i - 1, j),
        (i + 1, j),
        (i, j - 1),
        (i, j + 1),
        (i - 1, j - 1),
        (i - 1, j + 1),
        (i + 1, j - 1),
        (i + 1, j + 1),
    ]
    good_neighbors = [
        (m, n)
        for (m, n) in N_ij
        if (m >= 0 and m < ny and n >= 0 and n < nx and (m, n) not in obstacles)
    ]
    return good_neighbors





def select_dir(u, xk, obstacles=None, act_type="relu", rnd=True):
    # --- assuming (i,j) is not in self.obstacles ----
    # === construct valid local neighbors of (i,j) ===
    ny, nx = u.shape
    i = xk[0]
    j = xk[1]
    loc_grids = neighbors(xk, u.shape, obstacles=obstacles)
    loc_diff = [
        (u[i, j] - u[m, n]) / np.sqrt((i - m) ** 2 + (j - n) ** 2)
        for (m, n) in loc_grids
    ]

    # == call activation function to activate certain directions ==========================
    loc_diff = local_activation(act_type=act_type)(loc_diff)
    # print(loc_diff)
    # === compute local propbabilities by normalizing the activation ===
    if np.sum(loc_diff) > 0:
        loc_prob = loc_diff / np.sum(loc_diff)
        if rnd:
            idx = np.random.choice(len(loc_prob), 1, p=loc_prob)[0]
        else:
            idx = np.argmax(loc_prob)
        return loc_grids[idx]
    else:
        return None


# ======================================================================================

def local_activation(act_type="relu"):
    def relu(x):
        return np.maximum(0, x)
    def relu_a(x,a=-1e-3):
        return np.maximum(a, x) - a

    if act_type == "relu":
        return relu
    else:
        return relu_a


def main():
    nx = 256
    ny = 256
    
    #f = 1e-3*np.ones((ny, nx))
    #f = np.exp(7)*np.ones((ny,nx))
    #f = (nx**2)*np.ones((ny,nx))

    matplotlib.rcParams['figure.figsize'] = (8, 5.5)
    shasta_dem = rd.LoadGDAL("Baltimore.tiff")
    slope = rd.TerrainAttribute(shasta_dem, attrib='slope_riserun')

    f = 6*(np.exp(-3.5)) * abs((slope) + 0.05) * np.ones((ny, nx)) 

    obstacles = [(i,j) for i, j in itertools.product(range(ny//4, 3*ny//4),range(nx//2 - 10, nx//2 + 10))]
    
    for x in obstacles:
        f[x] = INF
    
    gamma = (ny//2-1, 0)
    omega = (ny//2-1, nx-1)
    u = INF*np.ones((ny, nx))
    u[gamma] = 0
    
    h = 1
    eps = 1e-5
    #--- computiong u ---
    u = fsm2d(u, f, h=h, eps=eps)
        
    #--- generating a path from gamma to omega
    act = 'relu_a'
    rnd = True
    
    path = path_gen(u, gamma, omega, obstacles=obstacles, act_type=act, rnd=rnd) 
    
    #--- visualizing ---
    v = u.copy()
    for x in obstacles:
        v[x] = 0
    v = 150*v/v.max()
    print(v.min(), v.max())
    for x in path:
        v[x]= 200
    for x in obstacles:
        v[x] = 255
    img = Image.fromarray(np.uint8(v))
    img.save(f"images/img.png")
    img.show()
    
if __name__ == "__main__":
    main()

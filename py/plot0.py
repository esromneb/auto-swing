import sys
sys.path.append('sigmath')

from sigmath import *


o = better_open_csv('data1.csv')



# ax = np.array(o.data[1])

if True:
	nplot(o.data[1], "AX")
	nplot(o.data[2], "AY")
	nplot(o.data[3], "AZ")
	nplot(o.data[4], "GX")
	nplot(o.data[5], "GY")
	nplot(o.data[6], "GZ")


if False:
	nplot(o.data[1], "AX")
	nplot(o.data[6], "GZ", False)
	nplotshow()



nplotshow()
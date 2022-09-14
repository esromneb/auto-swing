import sys
sys.path.append('sigmath')

from sigmath import *

# assumes idx is the last in the data
# removes all event rows, but moves the event to the next row
# gains the event up to make it easier to plot
def filter_events(data, idx, gain=1):
	found = []
	for row in range(len(data[0])):
		# print(data[0][row])
		if data[idx][row] != 0:
			found.append(row)
			# for j in range(idx):
			# 	print(data[j])
			# break
		# break
	# We have indices that we want to delete
	# reverse these for the loop below
	found.reverse()
	# print (found)

	# Now boost and modify the event column
	save = 0
	width = 0
	for row in range(len(data[0])):
		if data[idx][row] != 0:
			save = data[idx][row]
			width = 2

		if width:
			data[idx][row] = save*gain
			width -= 1

	# print(data[7])

	# Now that events have been modified
	# delete at the indices we found above
	for takeout in found:
		# print("deleting at " + str(takeout))
		for j in range(idx+1):
			del data[j][takeout]
			# print(j)


def mag(v):
	return np.sqrt(np.sum(np.square(v)))


o = better_open_csv('data3.csv')

# 7th colum has events in it
# gain the events up by 1k
filter_events(o.data, 7, 10000)

# ax = np.array(o.data[1])

if False:
	nplot(o.data[1], "AX")
	nplot(o.data[2], "AY")
	nplot(o.data[3], "AZ")
	nplot(o.data[4], "GX")
	nplot(o.data[5], "GY")
	nplot(o.data[6], "GZ")
	nplot(o.data[7], "Event")


if False:
	nplot(o.data[1], "AX")
	nplot(o.data[6], "GZ", False)

if False:

	myv = []
	for i in range(len(o.data[0])):
		myv.append( mag( [o.data[1][i], o.data[2][i], o.data[3][i] ] ))


	# nplot(o.data[1], "AX"   )
	# nplot(o.data[2], "AY"   , False)
	# nplot(o.data[3], "AZ"   , False)
	nplot(myv,       "as mag"   , False)
	nplot(o.data[7], "XYZ + Event", False)

# X,Z are the ones I want

if True:
	cdata = []
	for i in range(len(o.data[0])):
		print(str(o.data[1][i]) + ' ' + str(o.data[3][i]))
		cdata.append(complex(o.data[1][i], o.data[3][i]))
	cangle = np.angle(cdata)

	for i in range(len(cangle)):
		cangle[i] -= 1

	floatgain = 32768

	asint = [int(x) for x in np.floor(cangle*floatgain)]
	fstate = asint[0]
	fgain = 7209-2000

	asfilter = []
	for i in range(len(asint)):
		fstate = i32_fixed_iir_16(fstate, asint[i], fgain);
		asfilter.append(fstate)

	filterfloat = [x/floatgain for x in asfilter]

	nplot(cangle)
	nplot(filterfloat, "filter and not", False)

	# print()



if False:
	adata = [17746.0, 17191.0, 17098.0, 17520.0, 18072.0, 18224.0, 17992.0, 17260.0, 16831.0, 16711.0, 16830.0, 16746.0, 16630.0, 16507.0, 16495.0, 16569.0, 16984.0, 17161.0, 17094.0, 17335.0, 17671.0, 17424.0, 17626.0, 17422.0, 17323.0, 17405.0, 17576.0, 17251.0, 17205.0, 17539.0, 17736.0, 17985.0]
	adata = [int(x) for x in adata]
	# print("hi")
	fstate = adata[0]
	fgain = 7209
	for i in range(len(adata)):
		fstate = i32_fixed_iir_16(fstate, adata[i], fgain);
		print("%03d, %05d, %05d" % (i, adata[i], fstate))


nplotshow()

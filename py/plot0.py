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

if True:

	myv = []
	for i in range(len(o.data[0])):
		myv.append( mag( [o.data[1][i], o.data[2][i], o.data[3][i] ] ))


	# nplot(o.data[1], "AX"   )
	# nplot(o.data[2], "AY"   , False)
	# nplot(o.data[3], "AZ"   , False)
	nplot(myv,       "as mag"   , False)
	nplot(o.data[7], "XYZ + Event", False)


nplotshow()
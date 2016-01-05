import numpy as np

class DataRing():
	def __init__(self, capacity):
		self.array = np.zeros(2*capacity)
		self.capacity = capacity
		self.count = 0
		self.head = 0
		self.tail = 0

	def append(self, a):
		a = np.asarray(a)

		print 'add ', a
		print 'head =', self.head, '  tail =', self.tail, '  count =', self.count

		if a.size > self.capacity:
			# only append the last elements
			return

		if self.head+a.size <= self.capacity:
			self.array[self.head:self.head+a.size] = a[:]
			self.array[self.head+self.capacity:self.head+self.capacity+a.size] = a[:]
		else:
			first_size = self.capacity-self.head
			last_size = a.size-first_size
			self.array[self.head:self.head+first_size] = a[:first_size]
			self.array[0:last_size] = a[first_size:]
			self.array[self.head+self.capacity:self.head+self.capacity+first_size] = a[:first_size]
			self.array[self.capacity:self.capacity+last_size] = a[first_size:]

		self.count += a.size
		self.head = (self.head+a.size) % self.capacity

		if self.count > self.capacity:
			self.tail = self.head
			self.count = self.capacity

		print self.array

	def data(self):
		return self.array[self.tail:self.tail+self.count]

x = DataRing(4)
x.append([1, 2])
print x.data()
x.append([3])
print x.data()
x.append([4, 5])
print x.data()
x.append([6, 7, 8])
print x.data()
x.append([9, 10, 11])
print x.data()
x.append([12, 13, 14, 15])
print x.data()


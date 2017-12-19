import heapq
# https://github.com/marcoscastro/ucs/blob/master/
class PriorityQueue:

    def __init__(self):
        self._queue = []
        self._index = 0

    def insertThree(self, item1, item2, priority):
        heapq.heappush(self._queue, (priority, self._index, (item1, item2, priority)))
        self._index += 1

    def insert(self, item1, item2, item3, priority):
        heapq.heappush(self._queue, (priority, self._index, (item1, item2, item3, priority)))
        self._index += 1

    def remove(self):
        return heapq.heappop(self._queue)[-1]

    def is_empty(self):
        return len(self._queue) == 0
# queue = PriorityQueue()
# queue.insert('e', 9)
# queue.insert('a', 2)
# queue.insert('h', 13)
# queue.insert('e', 5)
# queue.insert('c', 11)
# print(queue.remove())
# print(queue.remove())
# print(queue.remove())
# print(queue.remove())
# print(queue.remove())
#
# queue = PriorityQueue()
# queue.insert('e',[], 8, 9)
# queue.insert('a',[], 11, 2)
# queue.insert('h',[], 15, 13)
# queue.insert('e',[], 100, 5)
# queue.insert('c',[], 78, 11)
# print(queue.remove())
# print(queue.remove())
# print(queue.remove())
# print(queue.remove())
# print(queue.remove())

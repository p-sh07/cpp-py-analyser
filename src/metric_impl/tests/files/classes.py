class Rectangle:
    def __init__(self, left_upper: tuple, right_lower: tuple):
        self.left_upper = left_upper
        self.right_lower = right_lower
        self.width = right_lower[0]-left_upper[0]
        self.height = right_lower[1]-left_upper[1]

    def area(self):
        return self.width * self.height

    def perimeter(self):
        return self.width * 2 + self.height * 2

    def move(self, x_change: int, y_change: int):
        corner = self.left_upper
        self.left_upper = (corner[0]+x_change, corner[1]+y_change)
        corner = self.right_lower
        self.right_lower = (corner[0]+x_change, corner[1]+y_change)

class Stopwatch:
    def __init__(self):
        self.seconds = 0
        self.minutes = 0

class LunchCard:
    def __init__(self, balance: float):
        self.balance = balance

    def __str__(self):
        pass

class TaskList:
    def __init__(self):
        self.tasks = []

    def add_task(self, name: str, priority: int):
        self.tasks.append((priority, name))

    def get_next(self):
        self.tasks.sort()
        # The list method pop removes and returns the last item in a list
        task = self.tasks.pop()
        # Return the name of the task (the second item in the tuple)
        return task[1]

    def number_of_tasks(self):
        return len(self.tasks)

    def clear_tasks(self):
        self.tasks = []
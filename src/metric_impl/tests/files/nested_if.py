def Testnestedif(x, y):
    if x > 0:
        if y > 0:
            assert x == y
        elif x < 0:
            while x < 10:
                print("stuff")
            return 0
        else:
            return 1
    return -1
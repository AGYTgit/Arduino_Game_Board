def set0(r, b):
  return r &= ~(1<<b)
def set1(r, b):
  return r |= (1<<b)
def test(r, b):
  return r & (1<<b)

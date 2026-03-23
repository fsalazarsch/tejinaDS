import sys

with open("input.txt", encoding="utf-8") as f:
    text = f.read()

seen = set()
for c in text:
    if not c.isspace() and c not in seen:
        print(f"0x{ord(c):04X}")
        seen.add(c)
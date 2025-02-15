hex_string = """
A9 00 85 F0 A9 01 85 F1
A2 00 A5 F1 9D 01 01 85
F2 18 65 F0 85 F1 A5 F2
85 F0 E8 E0 0E 30 EB
"""

# Split the string into individual hex values, remove whitespace, and convert them to the required format
hex_values = hex_string.split()
formatted_values = ", ".join(f"0x{value}" for value in hex_values)

# Output the result
print(f"u8 code[] = {{{formatted_values}, 0x22}};")

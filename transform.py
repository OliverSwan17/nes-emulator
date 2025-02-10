hex_string = """
A2 07 A9 17 0A A9 17 85
17 06 17 8D 27 01 0E 27
01 8D 27 01 1E 20 01 85
17 16 10
"""

# Split the string into individual hex values, remove whitespace, and convert them to the required format
hex_values = hex_string.split()
formatted_values = ", ".join(f"0x{value}" for value in hex_values)

# Output the result
print(f"u8 code[] = {{{formatted_values}, 0x22}};")

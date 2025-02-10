hex_string = """
A9 65 8D FF 01 A9 19 8D
00 01 A9 01 8D 00 02 6C
FF 01
"""

# Split the string into individual hex values, remove whitespace, and convert them to the required format
hex_values = hex_string.split()
formatted_values = ", ".join(f"0x{value}" for value in hex_values)

# Output the result
print(f"u8 code[] = {{{formatted_values}, 0x22}};")

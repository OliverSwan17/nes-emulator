hex_string = """
A9 FF 2A A9 43 2A A9 01
6A
"""

# Split the string into individual hex values, remove whitespace, and convert them to the required format
hex_values = hex_string.split()
formatted_values = ", ".join(f"0x{value}" for value in hex_values)

# Output the result
print(f"u8 code[] = {{{formatted_values}, 0x22}};")

hex_string = """
A9 21 85 25 A9 17 24 25
"""

# Split the string into individual hex values, remove whitespace, and convert them to the required format
hex_values = hex_string.split()
formatted_values = ", ".join(f"0x{value}" for value in hex_values)

# Output the result
print(f"u8 code[] = {{{formatted_values}, 0x22}};")

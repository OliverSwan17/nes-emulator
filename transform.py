hex_string = """
A9 17 85 17 A9 05 AA E4
17 A9 17 AA E4 17 A9 04
E4 04 E4 05
"""

# Split the string into individual hex values, remove whitespace, and convert them to the required format
hex_values = hex_string.split()
formatted_values = ", ".join(f"0x{value}" for value in hex_values)

# Output the result
print(f"{{{formatted_values}, 0x22}};")

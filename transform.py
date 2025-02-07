hex_string = """
A9 21 85 42 A9 17 E8 E8 E8 81 42
"""

# Split the string into individual hex values, remove whitespace, and convert them to the required format
hex_values = hex_string.split()
formatted_values = ", ".join(f"0x{value}" for value in hex_values)

# Output the result
print(f"{{{formatted_values}}}")

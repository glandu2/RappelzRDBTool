-- This file is a sample Lua script to describe a fake RDB database.

-- All standard libraries are available if needed (io, string, math, ...)
-- The "print" function can be used, its output is redirected to logs.
-- "print" output in the RDB tool GUI is available in the menu "Options -> Show log" with at least "Info" level.

-- Here is a description of the component of a Lua database description file:

-- Required
-- List all fields into the variable rdb.fields. One line with { } per field
-- Each field contains these parameters:
-- - The field name as a string. Example: "id"
--
-- - The field type, visibility flags and additional flags. All flags must be combined with | operator.
--   - The type can be:
--     - TYPE_BIT          : A bit field. The field can be either 1 or 0.
--     - TYPE_INT8         : A 8 bits integer. Values from -128 to 127.
--     - TYPE_INT16        : A 16 bits integer. Values from –32768 to 32767.
--     - TYPE_INT32        : A 32 bits integer. Values from –2147483648 to 2147483647.
--     - TYPE_INT64        : A 64 bits integer. Values from –9223372036854775808 to 9223372036854775807.
--     - TYPE_FLOAT32      : A 32 bits float value.
--     - TYPE_FLOAT64      : A 64 bits float value, also called double.
--     - TYPE_CHAR         : A string field of fixed maximum length. The string is terminated with \0 in the RDB file.
--                           The data length parameter contains the maximum length of the string.
--     - TYPE_VARCHAR_SIZE : A field containing the length of a variable length string of type TYPE_VARCHAR_STR or TYPE_NVARCHAR_STR.
--     - TYPE_VARCHAR_STR  : A string field with variable length. The length in the RDB file is defined by a TYPE_VARCHAR_SIZE field.
--     - TYPE_DECIMAL      : A decimal value with a fixed number of decimals. The data length parameter contains the number of decimals using MAKEINDEXEDVAR. 
--     - TYPE_NVARCHAR_STR : A localized string field with variable length. The length in the RDB file is defined by a TYPE_VARCHAR_SIZE field.
--                           A localized string field use NVARCHAR SQL type instead of VARCHAR.
--
--   - The visibility flag can be:
--     - TYPE_RDBIGNORE    : This flag makes the field not written nor read to or from a RDB file.
--     - TYPE_SQLIGNORE    : This flag makes the field not written nor read to or from a SQL table. If the table is created, it won't contain this column.
--     - TYPE_CSVIGNORE    : This flag makes the field not written nor read to or from a CSV file.
--     - TYPE_GUIIGNORE    : This flag makes the field hidden from the tool GUI interface.
--
--   - Additional flags are:
--     - TYPE_FLAG_KEY     : This flag cause this field to be a primary key when creating a SQL table
--     - TYPE_FLAG_SORT    : This flag add a ORDER BY clause on this field when reading data from a SQL table
--
-- - The field size and/or index
--   - This third parameter is optionnal. When not given, it is set to the default value 1.
--   - If set to a value, the field is an array. For example, a TYPE_INT8 field with size = 3 means an array of 3 8-bits integers.
--   - This parameter can be set using MAKEINDEXEDVAR for variable length string and decimal types:
--     - If the field is of type TYPE_VARCHAR_SIZE, T PE_VARCHAR_STR or TYPE_NVARCHAR_STR:
--       - The first argument of MAKEINDEXEDVAR is the index of the field.
--         - 2 field with the same index and types TYPE_VARCHAR_SIZE and TYPE_VARCHAR_STR/TYPE_NVARCHAR_STR must be defined for variable length string (like in db_string.rdb).
--
--       - The second argument is only used with type TYPE_VARCHAR_STR/TYPE_NVARCHAR_STR.
--         - It contains the maximum string size in the SQL database. This value is used in the SQL type (like varchar(64) for example).
--
--     - If the field is of type TYPE_DECIMAL:
--       - The first argument of MAKEINDEXEDVAR is the number of decimal (after the decimal dot).
--         - MAKEINDEXEDVAR(2, 1) means the field has 2 digits after the decimal dot, like so: 123456.12.
--
--       - The second argument is the field size (and should always be 1 for numbers)
rdb.fields = {
	{"id", TYPE_INT32},                      -- This line describe the first field, it is a 32 bits integer field named "id"
	{"level", TYPE_INT32 | TYPE_RDBIGNORE},  -- This "level" field is a 32 bits integer.
	                                         -- The TYPE_RDBIGNORE flag means this field is not in the RDB format of this file.

	{"price", TYPE_DECIMAL | TYPE_SQLIGNORE | TYPE_CSVIGNORE, MAKEINDEXEDVAR(2, 1)},
	                                         -- The "price" field is a decimal field with 2 decimals (as set by the first argument of MAKEINDEXEDVAR).
	                                         -- The second is the number of decimal values in this field, it should be always 1).
	                                         -- Possible value are 1.00, 504639.12, -165.50
	                                         -- TYPE_SQLIGNORE means this field does not exists in the SQL database.
	                                         -- The TYPE_CSVIGNORE flag means this field is not in the CSV format of this file.

	{"script", TYPE_CHAR | TYPE_CSVIGNORE, 128},   -- The "script" field is a string with a fixed size in the RDB file of 128 bytes.
	                                               -- The TYPE_CSVIGNORE flag means this field is not in the CSV format of this file.

	{"", TYPE_VARCHAR_SIZE, MAKEINDEXEDVAR(0, 1)}, -- This field contains the length of the "name" string field.
	                                               -- This is because the first argument of MAKEINDEXEDVAR of both fields are the same: 0

	{"", TYPE_VARCHAR_SIZE, MAKEINDEXEDVAR(1, 1)}, -- This field contains the length of the "value" string field.
	                                               -- This is because the first argument of MAKEINDEXEDVAR of both fields are the same: 1

	{"name", TYPE_VARCHAR_STR, MAKEINDEXEDVAR(0, 64)},     -- This "name" field is a variable length string with a maximum of 64 characters in the SQL database.
	                                                       -- This string is not localized (and should contains only ASCII) and map to a VARCHAR column in SQL.
	{"value", TYPE_NVARCHAR_STR, MAKEINDEXEDVAR(1, 3999)}, -- This "value" field is a variable length localized string a maximum of 3999 characters in the SQL database.
	                                                       -- This string is localized and can contain characters specific to the used language.
	                                                       -- It maps to a NVARCHAR column in SQL.
}

-- Optionnal
-- This define the order of the column in the SQL database.
-- It's used when creating the table.
-- It must be a list of columns with \0 at the end of the name.
rdb.sqlColumnOrder = table.concat({
	"id\0",
	"level\0",
	"name\0",
	"script\0",
	"value\0",
	"price\0",
})

-- Optionnal
-- This define the order of the column in the CSV format
-- It's used when creating and reading a CSV file
-- It must be a list of columns with \0 at the end of the name.
rdb.csvColumnOrder = table.concat({
	"id\0",
	"name\0",
	"level\0",
	"script\0",
	"value\0",
	"price\0",
})

-- Optionnal
-- This override the default RDB file name
-- The default is the name of this file without "xxDatabase.lua" and prepended with "db_"
-- For this exact file, it would be "db_sample".
rdb.defaultFileName = "db_sample"

-- Optionnal
-- This override the default SQL table name
-- The default is the name of this file without "xxDatabase.lua" and concat with "Resource"
-- For this exact file, it would be "SampleResource".
rdb.defaultTableName = "SampleResource"

-- Optionnal
-- This define a function called for each rows when reading or writing data to or from a file/table.
-- It can be used to define values for field not available in all formats
-- Arguments are:
--  - dst: The destination format. Can be:
--    - DF_RDB: when reading/writing a RDB file
--    - DF_CSV: when reading/writing a CSV file
--    - DF_SQL: when reading/writing a SQL table
--
--  - mode: the processing made: reading or writing. Possible values are:
--    - DCT_Read: when reading a file or table
--    - DCT_Write: when writing a file or table
--
--  - row: the row data. Contains each defined fields with values read or to be writen.
--    - When mode == DCT_Read, the row contains values just read.
--      - If values are modified, they will be changed as if they were read as-is and they will be the value shown in the GUI and used for subsequent writes.
--    - When mode == DCT_Write, the row contains values that will be written.
--      - If values are modified, they will be changed both in the target file/table and in the GUI.
--    - The row variable must be used like this:
--      - Given the declared field "level", the value for this field can be read/written with `row.level` or `row["level"]`.
--
--  - rowNum: the index of the current row to be processed. First row has index 0.
rdb.convertData = function (dst, mode, row, rowNum)
	-- Set a default value for the "script" field using "name" and "value" fields
	if mode == DCT_Read and dst == DF_CSV then
		if row.name == "script" and row.value ~= "" then
			row.script = row.value .. "()"
		end
	end

	-- Level is not in the RDB file. It's row index begining with 1.
	if mode == DCT_Read and dst == DF_RDB then
		row.level = rowNum + 1
		-- print can be used to debug code. The message will be shown in the "Show logs" menu with level "Info".
		print("Row index: " .. rowNum .. ", level computed: " .. row.level)
	end

	-- Price is only in RDB format and must absolutly be 15.02 for some reason, so set it when writing a RDB file
	if mode == DCT_Write and dst == DF_RDB then
		row.price = 15.02
	end
end

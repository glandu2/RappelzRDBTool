-- db_string.rdb with version 9.5.2 given by Ghost Informatics to be able to get its format
-- convertData implementation from Xijezu

rdb.fields = {
	{"code", TYPE_INT32},
	{"group_id", TYPE_INT32},
	{"value_size", TYPE_VARCHAR_SIZE | TYPE_SQLIGNORE, MAKEINDEXEDVAR(0, 1)},
	{"value", TYPE_NVARCHAR_STR, MAKEINDEXEDVAR(0, 3999)},
}

local strings_data

rdb.convertData = function (dst, mode, row, rowNum)
	if rowNum == 0 then
		strings_data = {}
	end

	if mode == DCT_Read and dst == DF_RDB then
		if row.value_size < 0 then
			row.value = strings_data[-row.value_size]
		else
			row.value_size = string.len(row.value) -- remove stuff after null terminated string
			strings_data[row.code] = row.value
		end
	elseif mode == DCT_Write and dst == DF_RDB then
		if strings_data[row.value] == nil then
			local stringLen = string.len(row.value)
			local mod = string.len(row.value) % 4
			if mod == 0 then
				row.value_size = stringLen + 4;
			elseif mod == 1 then
				row.value_size = stringLen + 3;
			elseif mod == 2 then
				row.value_size = stringLen + 2;
			elseif mod == 3 then
				row.value_size = stringLen + 5;
			end
			strings_data[row.value] = row.code
		else
			row.value_size = -strings_data[row.value]
		end
	end
end

rdb.getRdbDate = function (dst, mode, originalDate)
	-- override date with this one
	return os.time({year=2118, month=10, day=31})
end

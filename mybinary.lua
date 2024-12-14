package.loadlib('./mylibrary.so', 'mylibrary')()
--require('mylibrary')

contents = dir('./')
for _, filename in pairs(contents) do
  print(filename)
end

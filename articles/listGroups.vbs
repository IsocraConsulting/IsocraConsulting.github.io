rem listGroups.vbs
rem change the text below to have your computer and username
set user=GetObject("WinNT://DHAULAGIRI/denish")
for each group in user.groups
  msgbox group.name
next

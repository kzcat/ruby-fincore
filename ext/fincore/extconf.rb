require 'mkmf'

HEADERS = %w(unistd.h sys/mman.h sys/types.h sys/stat.h errno.h fcntl.h)

HEADERS.each do |header|
  have_header(header) || abort("#{header} is missing!")
end

create_makefile('fincore')

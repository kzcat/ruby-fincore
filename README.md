# ruby-fincore 
Ruby File class extension to determine pages of file contents being resident in memory(in core).

### Installation

To install:

```sh
$ gem install fincore
```

### Usage

```ruby
require 'fincore'

File.open('some/file') do |f|
  f.cached_pages # return number of pages that are cached
  f.total_pages  # return number of pages at total
end
```

### Instance methods for File class
* `cached_pages -> Integer`
* `total_pages -> Integer`

### Requirements
Your platform must support mincore system call.

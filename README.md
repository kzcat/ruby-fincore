# ruby-fincore 
Ruby File class extension to determine pages of file contents being resident in memory(in core).

## Installation

Add this line to your application's Gemfile:

    gem 'fincore'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install fincore

## Usage

```ruby
require 'fincore'

File.open('some/file') do |f|
  f.cached_pages # return number of pages that are cached
  f.total_pages  # return number of pages at total
end
```
work with `IO#advise`
```ruby
require 'fincore'

File.open('some/file') do |f|
  f.advise(:willneed) # tell the kernel file contents will be accessed.
  f.cached_pages
  => 620129
  
  f.advise(:dontneed) # tell the kernel no more access.
  f.cached_pages
  => 0
end
```


## Requirements
Platform must support `mincore(2)` system call.

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

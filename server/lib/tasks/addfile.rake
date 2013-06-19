desc "just for test"
task :AddFile => :environment do
  #Rake::Task['db:reset'].invoke
  #FileList.new('add_files/*').each do |file|
  #  puts "app/assets/images#{file.sub(/[^\/]+/,"")}"
  #end
  Dir.foreach("add_files") do |dir|
    if dir != "." and dir != ".." and File.directory?("add_files/#{dir}")
      @kind = Kind.find_by_name(dir)
      if @kind == nil # kind is not in Kind
        #create kind
        sh("mkdir","app/assets/images/#{dir}")
        add_file(Kind.create!(:name => dir))
      else #@kind is in Kind
        add_file(@kind)
      end
    end
  end
end

def add_file(kind)
  Dir.foreach("add_files/#{kind.name}") do |file|
    #add file to SQL
    if file != "." and file != ".." and File.file?("add_files/#{kind.name}/#{file}")
      Image.create(:name => file, :kind_id => kind.id, :location => "/assets/#{kind.name}/#{file}")
      sh("mv","add_files/#{kind.name}/#{file}","app/assets/images/#{kind.name}/")
    end
  end
end


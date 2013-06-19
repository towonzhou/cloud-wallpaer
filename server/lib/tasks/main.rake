desc "just for test"
task :data => :environment do
  #Rake::Task['db:reset'].invoke
  FileList.new('add_files/*').each do |file|
    puts "app/assets/images#{file.sub(/[^\/]+/,"")}"
  end
end

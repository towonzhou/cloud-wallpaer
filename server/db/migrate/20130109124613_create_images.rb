class CreateImages < ActiveRecord::Migration
  def change
    create_table :images do |t|
      t.string :name
      t.string :classification
      t.string :location
      t.integer :link,        :default => 0
      t.integer :unlink,      :default => 0

      t.timestamps
    end
  end
end

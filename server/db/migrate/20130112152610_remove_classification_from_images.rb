class RemoveClassificationFromImages < ActiveRecord::Migration
  def up
    remove_column :images, :classification
  end

  def down
    add_column :images, :classification, :string
  end
end

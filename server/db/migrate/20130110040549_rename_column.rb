class RenameColumn < ActiveRecord::Migration
  def up
    rename_column :images,:link,:like
    rename_column :images,:unlink,:unlike
  end

  def down
    rename_column :images,:like,:link
    rename_column :images,:unlike,:unlink
  end
end

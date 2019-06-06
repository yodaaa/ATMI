json.array!(@chats) do |chat|
  json.extract! chat, :id, :name, :mesg
  json.url chat_url(chat, format: :json)
end

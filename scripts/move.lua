local script = {}

SPACE = 32
W = 87
A = 65
S = 83
D = 68

function script.OnStart(entity)
  print("Cube script started.")
  print("Entity name: " .. Entity.getName(entity))
end

function script.OnUpdate(entity, deltaTime)
  local pos = Transform.getPosition(entity)
  local moveSpeed = 3 * deltaTime
  local jumpSpeed = 5 * deltaTime

  if Input.isKeyPressed(W) then
    pos.x = pos.x + moveSpeed
  elseif Input.isKeyPressed(S) then
    pos.x = pos.x - moveSpeed
  end
  if Input.isKeyPressed(A) then
    pos.z = pos.z + moveSpeed
  elseif Input.isKeyPressed(D) then
    pos.z = pos.z - moveSpeed
  end
  if Input.isKeyPressed(SPACE) then
    print("Space key pressed!")
    pos.y = pos.y + jumpSpeed
  end

  Transform.setPosition(entity, pos.x, pos.y, pos.z)
end

function script.OnDestroy(entity)
  print("Cube script destroyed.")
end

return script

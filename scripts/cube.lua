local script = {}

pressed = false

SPACE = 32

function script.OnStart(entity)
  print("Cube script started.")
  print("Entity ID: " .. Entity.getName(entity))
end

function script.OnUpdate(entity, deltaTime)
  if Input.isKeyPressed(SPACE) then
    print("Space key pressed!")
    Render.setColor(entity, 255, 0, 0, 255) -- Change color to red
    pressed = not pressed
  end
  if not pressed then
    Render.setColor(entity, 0 + math.random(0, 255), 100 + math.random(0, 155), 255 + math.random(0, 100), 255)
    Transform.setPosition(
      entity,
      Transform.getPosition(entity).x + math.random(-2, 2) * deltaTime,
      Transform.getPosition(entity).y + math.random(-2, 2) * deltaTime,
      Transform.getPosition(entity).z + math.random(-2, 2) * deltaTime
    )
  end
end

function script.OnDestroy(entity)
  print("Cube script destroyed.")
end

return script
